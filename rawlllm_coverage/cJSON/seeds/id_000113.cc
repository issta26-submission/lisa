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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls,
    // including creation, modification, printing/parsing, detaching/replacing items,
    // and proper cleanup. It returns 66 on successful completion.

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add primitive items to the object
    cJSON_AddStringToObject(root, "name", "Alice");
    cJSON_AddNumberToObject(root, "age", 30);

    // Add a boolean value (use CreateTrue/CreateFalse macros provided by cJSON)
    cJSON_AddItemToObject(root, "married", cJSON_CreateFalse());

    // Add a null value
    cJSON_AddItemToObject(root, "nickname", cJSON_CreateNull());

    // Create an array "children" and populate it
    cJSON *children = cJSON_CreateArray();
    if (!children) { cJSON_Delete(root); return 0; }

    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));

    cJSON *childObj = cJSON_CreateObject();
    if (!childObj) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(childObj, "name", "Charlie");
    cJSON_AddNumberToObject(childObj, "age", 5);
    cJSON_AddItemToArray(children, childObj);

    // Attach the array to root
    cJSON_AddItemToObject(root, "children", children);

    // Print the JSON to an unformatted string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return 0; }

    // Parse the printed string back into a cJSON structure (demonstrates Parse)
    cJSON *parsed = cJSON_Parse(printed);
    // free the printed string (cJSON_PrintUnformatted uses malloc)
    free(printed);
    if (!parsed) { cJSON_Delete(root); return 0; }

    // Access some items in the parsed JSON using case-sensitive lookup
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(nameItem) || (nameItem->valuestring == NULL)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    cJSON *childrenParsed = cJSON_GetObjectItemCaseSensitive(parsed, "children");
    if (!cJSON_IsArray(childrenParsed)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Demonstrate retrieving an array element from the parsed JSON
    cJSON *firstChildParsed = cJSON_GetArrayItem(childrenParsed, 0);
    (void)firstChildParsed; // we don't need to use it further here

    // Demonstrate detaching an item from the original root and deleting it
    cJSON *detachedNickname = cJSON_DetachItemFromObject(root, "nickname");
    if (detachedNickname) {
        // detachedNickname is no longer part of root; free it explicitly
        cJSON_Delete(detachedNickname);
    }

    // Replace the "age" item in the original root with a new number
    // cJSON_ReplaceItemInObject replaces and takes ownership of the new item
    cJSON_ReplaceItemInObject(root, "age", cJSON_CreateNumber(31));

    // Detach the first element from the children array (original root) and reattach it
    // as a top-level field "first_child"
    cJSON *detachedChild = cJSON_DetachItemFromArray(children, 0);
    if (detachedChild) {
        cJSON_AddItemToObject(root, "first_child", detachedChild);
        // detachedChild is now owned by root and will be freed with root
    }

    // Clean up parsed copy and the original document (this frees all remaining items)
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence succeeded
    return 66;
}