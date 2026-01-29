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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Note: this function assumes cJSON.h is included in the translation unit
    // and the program links against cJSON. It uses typical cJSON APIs.
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Create an array [1,2,3]
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3));

    // Attach array and some other items to root object
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "name", "example");

    // Create a nested object and attach
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return -1; }
    cJSON_AddStringToObject(nested, "inner", "value");
    cJSON_AddItemToObject(root, "nested", nested);

    // Duplicate the whole tree (deep copy)
    cJSON *copy = cJSON_Duplicate(root, 1); // 1 -> recurse / deep copy
    if (!copy) { cJSON_Delete(root); return -1; }

    // Modify the duplicate: add a new element to its "numbers" array
    cJSON *copy_numbers = cJSON_GetObjectItem(copy, "numbers");
    if (cJSON_IsArray(copy_numbers)) {
        cJSON_AddItemToArray(copy_numbers, cJSON_CreateNumber(42));
    } else {
        // if unexpected, cleanup and fail
        cJSON_Delete(root);
        cJSON_Delete(copy);
        return -1;
    }

    // Print the original (unformatted) to a string and free it
    char *out1 = cJSON_PrintUnformatted(root);
    if (out1) {
        // basic sanity check: non-empty
        if (out1[0] == '\0') {
            free(out1);
            cJSON_Delete(root);
            cJSON_Delete(copy);
            return -1;
        }
        free(out1);
    } else {
        cJSON_Delete(root);
        cJSON_Delete(copy);
        return -1;
    }

    // Print the duplicate (formatted) and free
    char *out2 = cJSON_Print(copy);
    if (out2) {
        free(out2);
    } else {
        cJSON_Delete(root);
        cJSON_Delete(copy);
        return -1;
    }

    // Validate some items from the original
    cJSON *name = cJSON_GetObjectItem(root, "name");
    if (!cJSON_IsString(name) || name->valuestring == NULL) {
        cJSON_Delete(root);
        cJSON_Delete(copy);
        return -1;
    }

    // Check first element of the numbers array
    cJSON *first_num = cJSON_GetArrayItem(arr, 0);
    if (!cJSON_IsNumber(first_num)) {
        cJSON_Delete(root);
        cJSON_Delete(copy);
        return -1;
    }

    // All operations succeeded; cleanup
    cJSON_Delete(root);
    cJSON_Delete(copy);

    return 66;
}