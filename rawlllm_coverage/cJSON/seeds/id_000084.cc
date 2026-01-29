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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic items
    if (!cJSON_AddStringToObject(root, "name", "ChatGPT")) { cJSON_Delete(root); return -2; }
    if (!cJSON_AddNumberToObject(root, "age", 3)) { cJSON_Delete(root); return -3; }

    // Create and populate an array, then attach to root
    cJSON *scores = cJSON_CreateArray();
    if (!scores) { cJSON_Delete(root); return -4; }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(77));
    cJSON_AddItemToObject(root, "scores", scores);

    // Duplicate the whole root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) { cJSON_Delete(root); return -5; }

    // Print duplicated JSON to a string
    char *printed = cJSON_PrintUnformatted(dup);
    if (!printed) { cJSON_Delete(dup); cJSON_Delete(root); return -6; }

    // Parse back the printed JSON
    cJSON *parsed = cJSON_Parse(printed);
    free(printed);
    if (!parsed) { cJSON_Delete(dup); cJSON_Delete(root); return -7; }

    // Validate parsed contents
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -8;
    }

    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age) || ((int)age->valuedouble != 3)) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -9;
    }

    cJSON *scores2 = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    if (!cJSON_IsArray(scores2) || cJSON_GetArraySize(scores2) != 3) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -10;
    }

    cJSON *first = cJSON_GetArrayItem(scores2, 0);
    if (!cJSON_IsNumber(first) || ((int)first->valuedouble != 95)) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -11;
    }

    // Demonstrate detach and reattach: remove "age" from root and re-add under "age_old"
    cJSON *detached_age = cJSON_DetachItemFromObject(root, "age");
    if (!detached_age) { cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -12; }
    cJSON_AddItemToObject(root, "age_old", detached_age);

    // Replace the "name" item in root
    cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("GPT-4"));

    // Duplicate the scores array and attach as a copy
    cJSON *scores_dup = cJSON_Duplicate(scores, 1);
    if (!scores_dup) { cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -13; }
    cJSON_AddItemToObject(root, "scores_copy", scores_dup);

    // Detach an array item (remove second score) and delete it
    cJSON *removed = cJSON_DetachItemFromArray(scores, 1); // removes the element with value 88
    if (!removed) { cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -14; }
    cJSON_Delete(removed);

    // Verify array size after removal
    if (cJSON_GetArraySize(scores) != 2) { cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return -15; }

    // Cleanup all created JSON structures
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // Indicate success as requested
    return 66;
}