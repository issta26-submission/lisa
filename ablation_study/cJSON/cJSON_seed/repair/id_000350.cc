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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"name\" : \"example\" , \"value\" : 3 , \"b\" : false } ";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *b_item = cJSON_GetObjectItem(root, "b");
    cJSON_bool b_false = cJSON_IsFalse(b_item);
    cJSON *arr = cJSON_CreateArray();
    cJSON *num7 = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(arr, 0, num7);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddNumberToObject(root, "computed_from_b", (double)b_false + 10.0);
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    double v = cJSON_GetNumberValue(val_item);
    cJSON_AddNumberToObject(root, "value_times_two", v * 2.0);

    // step 3: Operate and Validate
    double sum = v + (double)b_false;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_Delete(copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}