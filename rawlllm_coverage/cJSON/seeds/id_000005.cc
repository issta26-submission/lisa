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
//<ID> 5
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
        return -1; // allocation failed
    }

    // Add basic members (string and number)
    if (!cJSON_AddStringToObject(root, "name", "example")) {
        cJSON_Delete(root);
        return -1;
    }
    if (!cJSON_AddNumberToObject(root, "value", 42)) {
        cJSON_Delete(root);
        return -1;
    }

    // Create a nested object and add it to root
    cJSON *child = cJSON_CreateObject();
    if (!child) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddStringToObject(child, "nested_key", "nested_value");
    cJSON_AddItemToObject(root, "child", child); // ownership transferred to root

    // Create an array, populate it, and add to root
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));        // index 0
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));   // index 1
    cJSON *obj_in_array = cJSON_CreateObject();
    cJSON_AddStringToObject(obj_in_array, "k", "v");
    cJSON_AddItemToArray(arr, obj_in_array);                // index 2
    cJSON_AddItemToObject(root, "list", arr);               // ownership transferred to root

    // Add boolean and null values
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "maybe", cJSON_CreateNull());

    // Replace an existing item ("name" -> "updated")
    cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("updated"));

    // Detach and delete the first element of the array (index 0)
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    if (detached) {
        cJSON_Delete(detached); // free detached item
    }

    // Get current size of the array (after detachment)
    int arr_size = cJSON_GetArraySize(arr);

    // Duplicate the whole JSON tree (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1); // 1 => recurse (deep)
    if (!dup) {
        cJSON_Delete(root);
        return -1;
    }

    // Basic integrity checks on the duplicate
    cJSON *dup_name = cJSON_GetObjectItemCaseSensitive(dup, "name");
    cJSON *dup_list = cJSON_GetObjectItemCaseSensitive(dup, "list");
    if (!cJSON_IsString(dup_name) || !dup_list || cJSON_GetArraySize(dup_list) != arr_size) {
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -1;
    }

    // Remove an object member from the original (demonstrate deletion API)
    cJSON_DeleteItemFromObject(root, "maybe");

    // Check presence of an object item (flag)
    int has_flag = cJSON_HasObjectItem(root, "flag");
    (void)has_flag; // silence unused-variable warnings in test contexts

    // Clean up
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // If we reached here, the sequence succeeded
    return 66;
}