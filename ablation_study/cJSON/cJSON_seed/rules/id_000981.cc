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
//<ID> 981
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
    cJSON *values_arr = NULL;
    cJSON *second_item = NULL;
    cJSON *device_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    char json_text[] = " { \"device\" : \"cam01\" , \"active\" : false, \"values\" : [10, 20, 30] } ";
    size_t scratch_len = 32;
    double second_value = 0.0;
    int second_value_int = 0;
    int active_flag = 0;

    // step 2: Setup / Configure
    cJSON_Minify(json_text);
    parsed = cJSON_Parse(json_text);
    values_arr = cJSON_GetObjectItem(parsed, "values");
    second_item = cJSON_GetArrayItem(values_arr, 1);
    second_value = cJSON_GetNumberValue(second_item);
    second_value_int = (int)second_value;
    root = cJSON_CreateObject();
    device_item = cJSON_GetObjectItem(parsed, "device");
    cJSON_AddStringToObject(root, "device_copy", cJSON_GetStringValue(device_item));
    cJSON_AddNumberToObject(root, "second_value", second_value);
    cJSON_AddItemToObject(root, "report_active", cJSON_CreateFalse());

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = (char)('0' + (second_value_int % 10));
    active_flag = (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "report_active"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}