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
//<ID> 820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item1 = cJSON_CreateString("alpha");
    cJSON *str_item2 = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddItemToObject(root, "items", list);
    cJSON_AddItemToArray(list, num_item);
    cJSON_AddItemToArray(list, str_item1);
    cJSON_InsertItemInArray(list, 1, str_item2);
    cJSON_AddStringToObject(root, "temp", "to_be_deleted");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    int count = cJSON_GetArraySize(list);
    cJSON *first = cJSON_GetArrayItem(list, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *ver_node = cJSON_GetObjectItem(meta, "version");
    const char *ver_str = cJSON_GetStringValue(ver_node);
    (void)count;
    (void)first_val;
    (void)ver_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}