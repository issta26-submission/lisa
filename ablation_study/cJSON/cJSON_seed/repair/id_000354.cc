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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, num_item);
    cJSON_InsertItemInArray(array, 0, str_item);
    cJSON_AddItemToObject(root, "list", array);
    char json_buf[] = " { \"flag\": false, \"value\": 3.14 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double value = cJSON_GetNumberValue(value_item);

    // step 3: Operate and Validate
    double computed = value + (double)flag_is_false * -1.0;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}