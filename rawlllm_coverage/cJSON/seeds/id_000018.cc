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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Add simple items
    cJSON_AddStringToObject(root, "name", "Example User");
    cJSON_AddNumberToObject(root, "age", 30);
    // create a bool and add it
    cJSON_AddItemToObject(root, "is_student", cJSON_CreateBool(0)); // false

    // Create an array and populate it
    cJSON *scores = cJSON_CreateArray();
    if (!scores) {
        cJSON_Delete(root);
        return -2;
    }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88.5));
    cJSON_AddItemToObject(root, "scores", scores);

    // Create a nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) {
        cJSON_Delete(root);
        return -3;
    }
    cJSON_AddStringToObject(address, "street", "123 Main St");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Print formatted JSON to a string
    char *printed = cJSON_Print(root);
    if (!printed) {
        cJSON_Delete(root);
        return -4;
    }

    // Parse the printed JSON back to a new cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    // free the printed representation
    free(printed);
    if (!parsed) {
        cJSON_Delete(root);
        return -5;
    }

    // Inspect some fields in parsed object
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (cJSON_IsString(name_item) && name_item->valuestring) {
        // (example usage) access name_item->valuestring
    }

    cJSON *age_item = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (cJSON_IsNumber(age_item)) {
        // (example usage) access age_item->valuedouble or valueint
    }

    // Detach the original age item and free it, then replace with a new age
    cJSON *detached_age = cJSON_DetachItemFromObject(parsed, "age");
    if (detached_age) {
        cJSON_Delete(detached_age);
    }
    cJSON_AddNumberToObject(parsed, "age", 31);

    // Modify the first element of the scores array
    cJSON *parsed_scores = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    if (cJSON_IsArray(parsed_scores)) {
        // Replace first score with 100
        cJSON_ReplaceItemInArray(parsed_scores, 0, cJSON_CreateNumber(100));
    }

    // Replace a field inside the nested object
    cJSON *parsed_address = cJSON_GetObjectItemCaseSensitive(parsed, "address");
    if (cJSON_IsObject(parsed_address)) {
        cJSON_ReplaceItemInObject(parsed_address, "zip", cJSON_CreateNumber(54321));
    }

    // Optionally print the modified parsed object (uncomment to use)
    // char *modified = cJSON_PrintUnformatted(parsed);
    // if (modified) { puts(modified); free(modified); }

    // Clean up both cJSON trees
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    return 66;
}