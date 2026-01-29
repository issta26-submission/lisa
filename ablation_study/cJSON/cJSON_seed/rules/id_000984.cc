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
//<ID> 984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *extras = NULL;
    cJSON *snapshot_dup = NULL;
    cJSON *readings_arr = NULL;
    cJSON *first_reading = NULL;
    cJSON *flag_item = NULL;
    cJSON *false_item = NULL;
    char json_buf[] = " { \"sensor\": \"X\", \"readings\": [ 10, 20 ], \"state\": false } ";
    size_t json_len = (size_t)(sizeof(json_buf) - 1);
    char *printed = NULL;
    char *pre_buf = NULL;
    int pre_len = 128;
    cJSON_bool pre_ok = 0;
    double first_value = 0.0;

    // step 2: Setup / Configure
    cJSON_Minify(json_buf);
    parsed = cJSON_ParseWithLength(json_buf, json_len);
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "created_by", "fuzzer_unit");
    extras = cJSON_AddArrayToObject(root, "extras");
    cJSON_AddItemToArray(extras, cJSON_CreateNumber(123.0));
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    snapshot_dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "snapshot", snapshot_dup);

    // step 3: Operate / Validate
    readings_arr = cJSON_GetObjectItem(parsed, "readings");
    first_reading = cJSON_GetArrayItem(readings_arr, 0);
    first_value = cJSON_GetNumberValue(first_reading);
    printed = cJSON_PrintUnformatted(root);
    pre_buf = (char *)cJSON_malloc((size_t)pre_len);
    memset(pre_buf, 0, (size_t)pre_len);
    pre_ok = cJSON_PrintPreallocated(root, pre_buf, pre_len, 0);
    flag_item = cJSON_GetObjectItem(root, "flag");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(pre_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}