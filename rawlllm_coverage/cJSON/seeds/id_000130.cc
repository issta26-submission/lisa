#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Local includes are allowed inside a function scope; they ensure strcmp/free are available.
    #include <cstring> // for strcmp
    #include <cstdlib> // for free

    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add simple items: string, number, boolean
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 42);
    // Use cJSON_CreateTrue() to create a boolean true node and add it
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    // Create a nested object "address" with a city and zip
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 2; }
    cJSON_AddStringToObject(address, "city", "Metropolis");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Create an array "scores" and populate with numbers
    cJSON *scores = cJSON_CreateArray();
    if (!scores) { cJSON_Delete(root); return 3; }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(100));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88));
    cJSON_AddItemToObject(root, "scores", scores);

    // Deep-duplicate the whole JSON tree
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) { cJSON_Delete(root); return 4; }

    // Print the root object to a compact string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); cJSON_Delete(dup); return 5; }

    // Parse the printed string back to a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) { free(printed); cJSON_Delete(root); cJSON_Delete(dup); return 6; }

    // Verify the "name" item exists and is the expected string
    cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!pname || !cJSON_IsString(pname) || !pname->valuestring || std::strcmp(pname->valuestring, "example") != 0) {
        free(printed);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        return 7;
    }

    // Replace the age in the parsed object with a new value (99)
    cJSON_ReplaceItemInObject(parsed, "age", cJSON_CreateNumber(99));

    // Detach the "scores" array from parsed, then reattach it (demonstrates detach/attach)
    cJSON *detached_scores = cJSON_DetachItemFromObject(parsed, "scores");
    if (!detached_scores) {
        free(printed);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        return 8;
    }
    // Re-add the detached array back into parsed
    cJSON_AddItemToObject(parsed, "scores", detached_scores);

    // Work with the scores array: duplicate first element and append it
    cJSON *parr = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    if (!parr || !cJSON_IsArray(parr)) {
        free(printed);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        return 9;
    }
    int arr_size = cJSON_GetArraySize(parr);
    if (arr_size <= 0) {
        // nothing to duplicate, but still considered success for sequence
    } else {
        cJSON *first_item = cJSON_GetArrayItem(parr, 0);
        if (first_item) {
            cJSON *first_dup = cJSON_Duplicate(first_item, 1);
            if (first_dup) {
                cJSON_AddItemToArray(parr, first_dup);
            } else {
                free(printed);
                cJSON_Delete(root);
                cJSON_Delete(dup);
                cJSON_Delete(parsed);
                return 10;
            }
        }
    }

    // If there is a second element, replace it with a new number (55)
    if (cJSON_GetArraySize(parr) >= 2) {
        cJSON_ReplaceItemInArray(parr, 1, cJSON_CreateNumber(55));
    }

    // Detach the last item in the array and delete it explicitly
    int last_index = cJSON_GetArraySize(parr) - 1;
    if (last_index >= 0) {
        cJSON *last = cJSON_DetachItemFromArray(parr, last_index);
        if (last) {
            cJSON_Delete(last);
        }
    }

    // Clean up: free printed string and delete all cJSON trees we allocated
    free(printed);
    cJSON_Delete(root);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);

    // If we reached this point, the API sequence succeeded
    return 66;
}