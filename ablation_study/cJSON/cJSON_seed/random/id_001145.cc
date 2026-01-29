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
//<ID> 1145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    double v_count = 0.0;
    double v_elem1 = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object with metadata and a numeric member
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    label = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "label", label);
    count_item = cJSON_AddNumberToObject(root, "count", 42.0);

    // step 3: Configure - create an array, populate with numbers and attach to root
    values = cJSON_CreateArray();
    elem0 = cJSON_CreateNumber(1.0);
    elem1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, elem0);
    cJSON_AddItemToArray(values, elem1);
    cJSON_AddItemToObject(root, "values", values);

    // step 4: Operate - retrieve members and array elements, then detach an element via pointer
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    fetched_label = cJSON_GetObjectItem(fetched_meta, "label");
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_elem1 = cJSON_GetArrayItem(fetched_values, 1);
    v_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    v_elem1 = cJSON_GetNumberValue(fetched_elem1);
    detached = cJSON_DetachItemViaPointer(values, fetched_elem1);

    // step 5: Validate - serialize root, compute a small validation score, free serialization buffer
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(v_count == 42.0) + (int)(v_elem1 == 2.0) + (int)(detached != (cJSON *)0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete detached item and root to free all resources
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}