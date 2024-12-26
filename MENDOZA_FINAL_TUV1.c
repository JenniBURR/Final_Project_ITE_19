#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Function to check if a string is a valid Roman numeral
int isValidRoman(char roman[])
{
    for (int i = 0; i < strlen(roman); i++)
    {
        if (!(roman[i] == 'I' || roman[i] == 'V' || roman[i] == 'X' || roman[i] == 'L' || roman[i] == 'C' || roman[i] == 'D' || roman[i] == 'M'))
        {
            return 0; // Invalid character found
        }
    }
    return 1;
}

// Function to convert Roman numeral to decimal
int romanToDecimal(char roman[])
{
    int total = 0, value = 0, prevValue = 0;
    for (int i = strlen(roman) - 1; i >= 0; i--)
    {
        if (roman[i] == 'I')
            value = 1;
        else if (roman[i] == 'V')
            value = 5;
        else if (roman[i] == 'X')
            value = 10;
        else if (roman[i] == 'L')
            value = 50;
        else if (roman[i] == 'C')
            value = 100;
        else if (roman[i] == 'D')
            value = 500;
        else if (roman[i] == 'M')
            value = 1000;
        else
            return -1; // Invalid character

        if (value < prevValue)
            total -= value;
        else
            total += value;
        prevValue = value;
    }
    return total;
}

// Function to convert number to words
void numberToWords(int number, char *output)
{
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    int part, index = 0;
    char chunk[50];
    output[0] = '\0';

    if (number == 0)
    {
        strcpy(output, "Zero");
        return;
    }

    while (number > 0)
    {
        part = number % 1000;
        number /= 1000;
        if (part > 0)
        {
            chunk[0] = '\0';
            if (part / 100 > 0)
            {
                sprintf(chunk + strlen(chunk), "%s Hundred ", ones[part / 100]);
                part %= 100;
            }
            if (part >= 10 && part < 20)
            {
                sprintf(chunk + strlen(chunk), "%s", teens[part - 10]);
            }
            else
            {
                if (part / 10 > 0)
                {
                    sprintf(chunk + strlen(chunk), "%s", tens[part / 10]);
                    if (part % 10 > 0)
                        strcat(chunk, " ");
                }
                if (part % 10 > 0)
                    sprintf(chunk + strlen(chunk), "%s", ones[part % 10]);
            }
            if (thousands[index][0] != '\0')
            {
                sprintf(chunk + strlen(chunk), " %s", thousands[index]);
            }
            if (output[0] != '\0')
            {
                memmove(output + strlen(chunk) + 2, output, strlen(output) + 1);
                sprintf(output, "%s, %s", chunk, output + strlen(chunk) + 2);
            }
            else
            {
                strcpy(output, chunk);
            }
        }
        index++;
    }
}

// Function to perform operations
int calculate(int num1, int num2, char op)
{
    switch (op)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        return (num2 != 0) ? num1 / num2 : 0;
    default:
        return 0;
    }
}

// Function to process a line of input
void processLine(char *line, FILE *outputFile)
{
    char roman1[20], roman2[20], operator;
    char words[500];
    int num1, num2, result;

    if (sscanf(line, "%s %c %s", roman1, &operator, roman2) != 3)
    {
        printf("Invalid format: %s", line);
        return;
    }

    if (!isValidRoman(roman1) || !isValidRoman(roman2))
    {
        fprintf(outputFile, "Invalid Roman numeral(s): %s\n", line);
        return;
    }

    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    if (num1 == -1 || num2 == -1)
    {
        fprintf(outputFile, "Invalid Roman numeral(s): %s\n", line);
        return;
    }

    result = calculate(num1, num2, operator);

    // Print process to terminal
    printf("%d %c %d = %d\n", num1, operator, num2, result);

    // Convert result to words and write to file
    if (result < 0)
    {
        char temp[500];
        numberToWords(-result, temp); // Convert the positive part to words
        sprintf(words, "Negative %s", temp);
    }
    else
    {
        numberToWords(result, words);
    }

    fprintf(outputFile, "%s\n", words);
}

void displayMenu()
{
    printf("\nRoman Numeral Calculator\n");
    printf("1. Process input file\n");
    printf("2. Manual input\n");
    printf("3. Exit\n");
    printf("Select an option: ");
}

void manualInput()
{
    char roman1[20], roman2[20], operator;
    char words[500];
    int num1, num2, result;

    printf("Enter first Roman numeral: ");
    scanf("%s", roman1);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);
    printf("Enter second Roman numeral: ");
    scanf("%s", roman2);

    if (!isValidRoman(roman1) || !isValidRoman(roman2))
    {
        printf("Invalid Roman numeral(s).\n");
        return;
    }

    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    if (num1 == -1 || num2 == -1)
    {
        printf("Invalid Roman numeral(s).\n");
        return;
    }

    result = calculate(num1, num2, operator);

    printf("%d %c %d = %d\n", num1, operator, num2, result);

    numberToWords(result, words);
    printf("In words: %s\n", words);
}

int main()
{
    int choice;
    char line[100];
    FILE *inputFile, *outputFile;

    do
    {
        displayMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            inputFile = fopen("Input.txt", "r");
            if (inputFile == NULL)
            {
                printf("Error opening input file.\n");
                break;
            }

            outputFile = fopen("Output.txt", "w");
            if (outputFile == NULL)
            {
                printf("Error opening output file.\n");
                fclose(inputFile);
                break;
            }

            while (fgets(line, sizeof(line), inputFile))
            {
                processLine(line, outputFile);
            }

            fclose(inputFile);
            fclose(outputFile);
            printf("Processing complete. Check Output.txt for results.\n");
            break;

        case 2:
            manualInput();
            break;

        case 3:
            printf("Exiting program. Goodbye!\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}
