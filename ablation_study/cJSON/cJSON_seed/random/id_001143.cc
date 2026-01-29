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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *fetched_count = (cJSON *)0;
    cJSON *fetched_list = (cJSON *)0;
    cJSON *fetched_s1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    char *detached_str = (char *)0;
    double number_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, number member, and an array of strings
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 100.0);
    list = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, s0);
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToObject(root, "list", list);

    // step 3: Operate - retrieve numeric member and an array element, then detach the element
    fetched_count = cJSON_GetObjectItem(root, "count");
    number_val = cJSON_GetNumberValue(fetched_count);
    fetched_list = cJSON_GetObjectItem(root, "list");
    fetched_s1 = cJSON_GetArrayItem(fetched_list, 1);
    detached = cJSON_DetachItemViaPointer(fetched_list, fetched_s1);
    detached_str = cJSON_GetStringValue(detached);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - compute a small validation score based on retrieved values
    validation_score = (int)(number_val == 100.0) + (int)(detached_str != (char *)0);

    // step 5: Cleanup - free printed buffer, delete detached item and root (which frees remaining)
    cJSON_free((void *)printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}