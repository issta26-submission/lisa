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
//<ID> 1279
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *created_num = NULL;
    cJSON *null_item = NULL;
    cJSON *str_item = NULL;
    cJSON *tmp = NULL;
    const char *str_val = NULL;
    double created_val = 0.0;
    double direct_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    created_num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "created_number", created_num);
    cJSON_AddNumberToObject(root, "direct_number", 7.25);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "created_number");
    created_val = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "direct_number");
    direct_val = cJSON_GetNumberValue(tmp);
    str_item = cJSON_GetObjectItem(root, "greeting");
    str_val = cJSON_GetStringValue(str_item);
    (void)created_val;
    (void)direct_val;
    (void)str_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}