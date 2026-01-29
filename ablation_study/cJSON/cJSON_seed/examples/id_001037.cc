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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item1 = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("alpha");
    cJSON *value1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(item1, "name", name1);
    cJSON_AddItemToObject(item1, "value", value1);
    cJSON_AddItemToObject(root, "item1", item1);

    // step 2: Configure
    cJSON *item2 = cJSON_Duplicate(item1, 1);
    cJSON_AddItemToObject(root, "item2", item2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "ok", flag);

    // step 3: Operate & Validate
    cJSON *got1 = cJSON_GetObjectItem(root, "item1");
    cJSON *got2 = cJSON_GetObjectItem(root, "item2");
    cJSON *got_name1 = cJSON_GetObjectItem(got1, "name");
    cJSON *got_name2 = cJSON_GetObjectItem(got2, "name");
    cJSON_bool names_equal = cJSON_Compare(got_name1, got_name2, 1);
    cJSON_bool items_equal = cJSON_Compare(got1, got2, 1);
    cJSON_bool ok_flag = cJSON_IsTrue(flag);
    cJSON *value_item = cJSON_GetObjectItem(got1, "value");
    double val = cJSON_GetNumberValue(value_item);
    cJSON *val_copy = cJSON_CreateNumber(val);
    cJSON_AddItemToObject(root, "value_copy", val_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}