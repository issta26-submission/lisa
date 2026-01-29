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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"values\" : [ 1, 2, 3 ], \"flag\" : false } ";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *flag = cJSON_GetObjectItem(root, "flag");

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(values, 1, new_num);
    cJSON *added = cJSON_AddNumberToObject(root, "added", 3.14);

    // step 3: Operate
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double computed = first_val + (double)(1 - flag_is_false) * 10.0;
    cJSON *computed_item = cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    (void)added;
    (void)computed_item;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}