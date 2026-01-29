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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *manual_num = NULL;
    cJSON *null_item = NULL;
    cJSON *item = NULL;
    char *printed = NULL;
    const char *status_str = NULL;
    double answer_val = 0.0;
    double pi_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "answer", 42.0);
    manual_num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", manual_num);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    cJSON_AddStringToObject(root, "status", "active");
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    item = cJSON_GetObjectItem(root, "status");
    status_str = cJSON_GetStringValue(item);
    item = cJSON_GetObjectItem(root, "answer");
    answer_val = cJSON_GetNumberValue(item);
    item = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(item);
    (void)status_str;
    (void)answer_val;
    (void)pi_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}