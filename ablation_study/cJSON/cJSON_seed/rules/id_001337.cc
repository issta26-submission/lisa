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
//<ID> 1337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *dup_str = NULL;
    cJSON *num_doubled = NULL;
    cJSON *num_sum = NULL;
    const cJSON *got_num = NULL;
    double num_val = 0.0;
    double doubled_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddItemToObject(root, "text", str_item);
    got_num = cJSON_GetObjectItem(root, "num");
    num_val = cJSON_GetNumberValue(got_num);
    num_doubled = cJSON_CreateNumber(num_val * 2.0);
    cJSON_AddItemToObject(root, "num_doubled", num_doubled);

    // step 3: Operate / Validate
    doubled_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "num_doubled"));
    num_sum = cJSON_CreateNumber(num_val + doubled_val);
    cJSON_AddItemToObject(root, "num_sum", num_sum);
    dup_str = cJSON_Duplicate(cJSON_GetObjectItem(root, "text"), 1);
    cJSON_AddItemToObject(root, "text_copy", dup_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}