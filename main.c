#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Convert the cipher from hex to decimal. */
void hex_to_decimal(char *cipher, int *numbers) {
    char num[3];
    num[2] = '\0';

    for (int i = 0, j = 0; i < 31; i++, j += 2) {
        num[0] = cipher[j];
        num[1] = cipher[j + 1];
        numbers[i] = (int) strtol(num, NULL, 16);
    }
}

int main(void) {
    int space, max, cipher_dec[7][31], key[31];

    char *cipher_hex[7] = {"BB3A65F6F0034FA957F6A767699CE7FABA855AFB4F2B520AEAD612944A801E",
                           "BA7F24F2A35357A05CB8A16762C5A6AAAC924AE6447F0608A3D11388569A1E",
                           "A67261BBB30651BA5CF6BA297ED0E7B4E9894AA95E300247F0C0028F409A1E",
                           "A57261F5F0004BA74CF4AA2979D9A6B7AC854DA95E305203EC8515954C9D0F",
                           "BB3A70F3B91D48E84DF0AB702ECFEEB5BC8C5DA94C301E0BECD241954C831E",
                           "A6726DE8F01A50E849EDBC6C7C9CF2B2A88E19FD423E0647ECCB04DD4C9D1E",
                           "BC7570BBBF1D46E85AF9AA6C7A9CEFA9E9825CFD5E3A0047F7CD009305A71E"};

    for (int i = 0; i < 7; i++)
        hex_to_decimal(cipher_hex[i], cipher_dec[i]);

    /* All combinations of messages to check for a space character. */
    int combinations[21][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
                               {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 3},
                               {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6},
                               {4, 5}, {4, 6}, {5, 6}};

    /* Try to find the key for every column. */
    for (int column = 0; column < 31; column++) {
        int freq[7] = {0};

        /* Try every combination with an XOR test. */
        for (int comb = 0; comb < 21; comb++) {
            int i = combinations[comb][0];
            int j = combinations[comb][1];

            /* Increment the frequency counter if one of the characters
             * is a space.
             */
            if ((cipher_dec[i][column] ^ cipher_dec[j][column]) >= 64) {
                freq[i]++;
                freq[j]++;
            }
        }

        /* The space character will be the character which succeeded the most
         * during the XOR test.
         */
        space = 0, max = 0;
        for (int i = 0; i < 7; i++) {
            if (freq[i] > max) {
                max = freq[i];
                space = i;
            }
        }

        /* If the space character was found, calculate the key. */
        key[column] = max > 0 ? cipher_dec[space][column] ^ ' ' : 0;
    }

    /* These are the keys we had to guess. */
    key[0] = 242;
    key[6] = 35;
    key[8] = 57;
    key[10] = 206;
    key[17] = 224;
    key[20] = 42;
    key[29] = 238;
    key[30] = 48;

    printf("Found key: ");
    for (int i = 0; i < 31; i++)
        printf("%02X", key[i]);

    printf("\n\nPlain text message:\n");
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 31; j++)
            putchar(cipher_dec[i][j] ^ key[j]);
        printf("\n");
    }

    return EXIT_SUCCESS;
}
