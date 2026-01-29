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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *obj_item = cJSON_CreateObject();
    cJSON_AddStringToObject(obj_item, "name", "example");
    cJSON_AddNumberToObject(obj_item, "value", 7.5);
    cJSON *str_item = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_InsertItemInArray(data, 0, num_item);
    cJSON_InsertItemInArray(data, 1, obj_item);
    cJSON_AddItemToArray(data, str_item);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    double first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(data, 0));
    cJSON *got_obj = cJSON_GetArrayItem(data, 1);
    char *got_name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(got_obj, "name"));
    (void)first_num;
    (void)got_name;
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}