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
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\": {\"tags\": [\"alpha\",\"beta\"]}, \"data\": {\"name\": \"sample\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *name_item = cJSON_GetObjectItem(data, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "extracted_name", name_value);
    cJSON_bool has_name = cJSON_HasObjectItem(data, "name");
    cJSON_AddBoolToObject(root, "data_has_name", has_name);
    const char *arr_strings[] = { "one", "two", "three" };
    cJSON *str_arr = cJSON_CreateStringArray(arr_strings, 3);
    cJSON_AddItemToObject(root, "new_strings", str_arr);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "meta");
    cJSON_DeleteItemFromObject(root, "new_strings");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}