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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "values", values);
    cJSON *first_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, first_num);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(cfg, "label", label);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 1.0);
    char *ignored_set = cJSON_SetValuestring(label, "updated");
    (void)ignored_set;
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "count", 5.0);
    cJSON *insert_num = cJSON_CreateNumber(2.718);
    cJSON_InsertItemInArray(values, 1, insert_num);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_values = cJSON_GetObjectItem(got_cfg, "values");
    cJSON *it0 = cJSON_GetArrayItem(got_values, 0);
    cJSON *it1 = cJSON_GetArrayItem(got_values, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double total = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", total);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}