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
//<ID> 827
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
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *list = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddItemToObject(data, "values", list);
    cJSON_AddItemToArray(list, num_item);
    cJSON_InsertItemInArray(list, 0, str_item);
    cJSON_AddItemToObject(data, "flag", true_item);

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *got_version = cJSON_GetObjectItemCaseSensitive(got_meta, "version");
    const char *version_str = cJSON_GetStringValue(got_version);
    cJSON *got_values = cJSON_GetObjectItem(data, "values");
    cJSON *second_value = cJSON_GetArrayItem(got_values, 1);
    double extracted_number = cJSON_GetNumberValue(second_value);
    (void)version_str;
    (void)extracted_number;
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    char *after_delete = cJSON_PrintUnformatted(root);
    cJSON_free(after_delete);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}