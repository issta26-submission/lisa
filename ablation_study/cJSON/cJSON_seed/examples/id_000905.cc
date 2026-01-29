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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_bool flag_value = 1;

    // step 2: Configure
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON_AddBoolToObject(root, "flag", flag_value);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    char sample_json[] = " { \"a\" : 21 , \"b\" : true } ";
    cJSON_Minify(sample_json);
    cJSON *parsed = cJSON_Parse(sample_json);
    cJSON *a_item = cJSON_GetObjectItem(parsed, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON *doubled = cJSON_CreateNumber(a_val * 2.0);
    cJSON_AddItemToObject(root, "a_times_two", doubled);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}