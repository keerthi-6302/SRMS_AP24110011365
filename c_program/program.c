#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[100];
    float marks;
};

char currentRole[20];
char currentUser[50];

/* FUNCTIONS */
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

char calculateGrade(float marks);
float calculateGPA(float marks);

/* =======================================================
                        MAIN
======================================================= */

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nAccess Denied. Exiting...\n");
    }
    return 0;
}

/* =======================================================
                    LOGIN SYSTEM
======================================================= */

int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];
    int attempts = 3;

    while (attempts--) {

        printf("\n=========== LOGIN ===========\n");

        printf("Enter Username: ");
        scanf(" %[^\n]", username);

        printf("Enter Password: ");
        int i = 0;
        char ch;
        while ((ch = getch()) != '\r') {
            if (ch == '\b' && i > 0) {
                i--;
                printf("\b \b");
            } else if (ch != '\b') {
                password[i++] = ch;
                printf("*");
            }
        }
        password[i] = '\0';
        printf("\n");

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credentials file missing!\n");
            return 0;
        }

        int loginSuccess = 0;

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcasecmp(username, fileUser) == 0 &&
                strcmp(password, filePass) == 0) {

                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                loginSuccess = 1;
                break;
            }
        }

        fclose(fp);

        if (loginSuccess) {
            printf("\nLogin Successful! Welcome %s!\n", currentUser);
            return 1;
        }

        printf("\nIncorrect username or password! Attempts left: %d\n", attempts);
    }

    return 0;
}

/* =======================================================
                          MENUS
======================================================= */

void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

void adminMenu() {
    int ch;

    do {
        printf("\n======== ADMIN MENU ========\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

void userMenu() {
    int ch;

    do {
        printf("\n======== USER MENU ========\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

void staffMenu() {
    int ch;

    do {
        printf("\n======== STAFF MENU ========\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

void guestMenu() {
    int ch;

    do {
        printf("\n======== GUEST MENU ========\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* =======================================================
                        ADD STUDENT
======================================================= */

int isDuplicate(int roll, char name[]) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;

    char line[200];
    struct Student s;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%[^|]|%f", &s.roll, s.name, &s.marks) == 3) {
            if (s.roll == roll || strcasecmp(s.name, name) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void addStudent() {
    struct Student s;

    printf("Enter Roll: ");
    scanf("%d", &s.roll);

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);

    if (isDuplicate(s.roll, s.name)) {
        printf("\n❌ ERROR: Duplicate Roll or Name found!\n");
        return;
    }

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    FILE *fp = fopen(STUDENT_FILE, "a");
    fprintf(fp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("\nStudent Added Successfully!\n");
}

/* =======================================================
                      DISPLAY STUDENTS
======================================================= */

void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No student file found!\n");
        return;
    }

    char line[200];
    struct Student s;

    printf("\nROLL | NAME | MARKS | GRADE | GPA\n");
    printf("----------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%[^|]|%f", &s.roll, s.name, &s.marks) == 3) {
            printf("%d | %s | %.2f | %c | %.2f\n",
                   s.roll, s.name, s.marks,
                   calculateGrade(s.marks),
                   calculateGPA(s.marks));
        }
    }
    fclose(fp);
}

/* =======================================================
                     SEARCH STUDENT (FIXED)
======================================================= */

void searchStudent() {
    int choice;
    char line[200];
    struct Student s;
    int found = 0;
    char searchName[100];
    int searchRoll;

    printf("Search by: 1.Roll  2.Name : ");
    scanf("%d", &choice);

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("Student file missing!\n"); return; }

    if (choice == 1) {
        printf("Enter Roll: ");
        scanf("%d", &searchRoll);
    }
    else if (choice == 2) {
        printf("Enter Name: ");
        scanf(" %[^\n]", searchName);
    }
    else {
        printf("Invalid choice!\n");
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {

        if (sscanf(line, "%d|%[^|]|%f", &s.roll, s.name, &s.marks) != 3)
            continue;

        if ((choice == 1 && s.roll == searchRoll) ||
            (choice == 2 && strcasecmp(s.name, searchName) == 0)) {

            printf("\nFOUND → Roll: %d | Name: %s | Marks: %.2f | Grade: %c | GPA: %.2f\n",
                   s.roll, s.name, s.marks,
                   calculateGrade(s.marks),
                   calculateGPA(s.marks));
            found = 1;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nStudent Not Found!\n");
}

/* =======================================================
                    UPDATE STUDENT
======================================================= */

void updateStudent() {
    int found = 0, choice;
    struct Student s;
    int searchRoll;
    char searchName[100];
    char line[200];

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) { printf("File error!\n"); return; }

    printf("Update by: 1.Roll  2.Name : ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Roll: ");
        scanf("%d", &searchRoll);
    }
    else if (choice == 2) {
        printf("Enter Name: ");
        scanf(" %[^\n]", searchName);
    }
    else {
        printf("Invalid choice!\n");
        fclose(fp); fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%[^|]|%f", &s.roll, s.name, &s.marks) != 3)
            continue;

        if ((choice == 1 && s.roll == searchRoll) ||
            (choice == 2 && strcasecmp(s.name, searchName) == 0)) {

            found = 1;
            int uchoice;
            printf("Update: 1.Name 2.Marks 3.Both: ");
            scanf("%d", &uchoice);

            if (uchoice == 1) {
                printf("New Name: ");
                scanf(" %[^\n]", s.name);
            }
            else if (uchoice == 2) {
                printf("New Marks: ");
                scanf("%f", &s.marks);
            }
            else if (uchoice == 3) {
                printf("New Name: ");
                scanf(" %[^\n]", s.name);
                printf("New Marks: ");
                scanf("%f", &s.marks);
            }
        }

        fprintf(temp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("\nStudent Updated Successfully!\n");
    else
        printf("\nStudent Not Found!\n");
}

/* =======================================================
                    DELETE STUDENT
======================================================= */

void deleteStudent() {
    int searchRoll, found = 0;
    struct Student s;
    char line[200];

    printf("Enter Roll to Delete: ");
    scanf("%d", &searchRoll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) { printf("File error!\n"); return; }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%[^|]|%f", &s.roll, s.name, &s.marks) != 3)
            continue;

        if (s.roll != searchRoll)
            fprintf(temp, "%d|%s|%.2f\n", s.roll, s.name, s.marks);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("\nStudent Deleted Successfully!\n");
    else
        printf("\nStudent Not Found!\n");
}

/* =======================================================
                GRADE + GPA CALCULATOR
======================================================= */

char calculateGrade(float marks) {
    if (marks >= 90) return 'A';
    if (marks >= 75) return 'B';
    if (marks >= 60) return 'C';
    if (marks >= 50) return 'D';
    return 'F';
}

float calculateGPA(float marks) {
    if (marks >= 90) return 4.0;
    if (marks >= 80) return 3.5;
    if (marks >= 70) return 3.0;
    if (marks >= 60) return 2.5;
    if (marks >= 50) return 2.0;
    return 0.0;
}