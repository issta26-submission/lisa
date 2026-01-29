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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Add simple items
    cJSON_AddStringToObject(root, "name", "test");
    cJSON_AddNumberToObject(root, "version", 1);

    // Create and add a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) {
        cJSON_Delete(root);
        return -2;
    }
    cJSON_AddStringToObject(nested, "inner", "value");
    cJSON_AddItemToObject(root, "nested", nested); // ownership transferred to root

    // Create an array and add items
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return -3;
    }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));
    cJSON_AddItemToObject(root, "items", arr); // ownership transferred to root

    // Print unformatted JSON to a string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) {
        cJSON_Delete(root);
        return -4;
    }

    // Parse the printed JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    free(printed);
    if (!parsed) {
        cJSON_Delete(root);
        return -5;
    }

    // Validate some fields from the parsed JSON
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -6;
    }
    cJSON *version = cJSON_GetObjectItemCaseSensitive(parsed, "version");
    if (!cJSON_IsNumber(version)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -7;
    }

    // Detach the nested object from parsed (transfers ownership to caller)
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "nested");
    if (!detached) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -8;
    }

    // Replace the "version" value in parsed with a new number
    cJSON_ReplaceItemInObject(parsed, "version", cJSON_CreateNumber(2));

    // Duplicate the parsed structure (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, true);
    if (!dup) {
        cJSON_Delete(detached);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -9;
    }

    // Print the duplicate in formatted form and free the returned string
    char *pretty = cJSON_Print(dup);
    if (pretty) {
        free(pretty);
    }

    // Clean up detached/duplicated/parsed/root trees
    cJSON_Delete(detached);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Demonstrate cJSON_Minify on an input string and parse the result
    char spaced[] = " { \"a\" : 1 , \"b\" : [ true , false , null ] } ";
    cJSON_Minify(spaced);
    cJSON *minip = cJSON_Parse(spaced);
    if (!minip) {
        return -10;
    }
    cJSON_Delete(minip);

    // If we reach here, the sequence succeeded
    return 66;
}