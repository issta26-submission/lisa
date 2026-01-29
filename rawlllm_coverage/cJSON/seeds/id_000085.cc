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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Add simple items
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);

    // Create an array and add numbers to it
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return -2;
    }
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3));

    // Duplicate the whole tree (deep copy)
    cJSON *clone = cJSON_Duplicate(root, 1); // 1 => recurse (deep)
    if (!clone) {
        cJSON_Delete(root);
        return -3;
    }

    // Replace an item in the original object
    cJSON_ReplaceItemInObject(root, "age", cJSON_CreateNumber(31));

    // Detach an item (takes it out of the object without deleting it)
    cJSON *detached = cJSON_DetachItemFromObject(root, "name");
    // We can inspect detached if needed, then must delete it ourselves
    if (detached) {
        if (cJSON_IsString(detached) && detached->valuestring) {
            // example use: (no-op)
        }
    }

    // Print the modified original object (unformatted) and free the print buffer
    char *printed = cJSON_PrintUnformatted(root);
    if (printed) {
        free(printed);
        printed = NULL;
    }

    // Parse a JSON string and add the result into the original object (ownership transfers)
    const char *json_text = "{\"new\":true, \"count\":42}";
    cJSON *parsed = cJSON_Parse(json_text);
    if (parsed) {
        // Basic sanity check on parsed content
        cJSON *newflag = cJSON_GetObjectItemCaseSensitive(parsed, "new");
        if (!newflag || !cJSON_IsTrue(newflag)) {
            // malformed as per our expectation -> clean up
            cJSON_Delete(parsed);
            cJSON_Delete(clone);
            if (detached) cJSON_Delete(detached);
            cJSON_Delete(root);
            return -4;
        }
        // Add parsed object into root; ownership of parsed is now with root
        cJSON_AddItemToObject(root, "parsed", parsed);
        parsed = NULL; // avoid accidental double-delete
    }

    // Create a shallow duplicate of the previous clone to test duplicate with recurse=0
    cJSON *clone2 = cJSON_Duplicate(clone, 0); // shallow duplicate
    if (clone2) {
        cJSON_Delete(clone2);
        clone2 = NULL;
    }

    // Access the numbers array and detach the first element from it (ownership returned)
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    if (!numbers || !cJSON_IsArray(numbers)) {
        cJSON_Delete(clone);
        if (detached) cJSON_Delete(detached);
        cJSON_Delete(root);
        return -5;
    }
    cJSON *first = cJSON_DetachItemFromArray(numbers, 0);
    if (first) {
        cJSON_Delete(first);
        first = NULL;
    }

    // Clean up: delete the duplicate and the detached item we still own, then delete the root (which also deletes parsed)
    cJSON_Delete(clone);
    clone = NULL;
    if (detached) {
        cJSON_Delete(detached);
        detached = NULL;
    }
    cJSON_Delete(root);
    root = NULL;

    // Success
    return 66;
}