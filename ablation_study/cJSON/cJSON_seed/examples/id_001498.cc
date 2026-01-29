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
//<ID> 1498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 30.0);
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(85.5));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(92.0));
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON *scores_ref = cJSON_CreateArrayReference(scores);
    cJSON_AddItemToObject(root, "scores_ref", scores_ref);
    const cJSON *name_item = cJSON_GetObjectItem(person, "name");
    char *name_value = (char*)cJSON_GetStringValue(name_item);
    char *printed_root = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal_before = cJSON_Compare(root, duplicate, 1);
    cJSON *detached = cJSON_DetachItemFromObject(root, "scores_ref");
    char *printed_detached = cJSON_PrintUnformatted(detached);
    (void)name_value;
    (void)equal_before;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}