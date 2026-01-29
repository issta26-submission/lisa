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
//<ID> 1412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON_AddItemToObject(meta, "detail", cJSON_CreateString("detail_text"));
    cJSON_AddItemReferenceToArray(values, num_item);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToArray(values, meta_dup);

    // step 3: Operate & Validate
    cJSON *found_num = cJSON_GetObjectItem(root, "answer");
    double val = cJSON_GetNumberValue(found_num);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON *copied_answer = cJSON_GetObjectItem(root_copy, "answer");
    double copied_val = cJSON_GetNumberValue(copied_answer);
    char *snapshot = cJSON_PrintUnformatted(root_copy);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}