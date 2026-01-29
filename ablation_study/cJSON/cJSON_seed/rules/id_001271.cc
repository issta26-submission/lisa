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
//<ID> 1271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *null_item = cJSON_CreateNull();
    const double added_val = 42.0;
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_message = NULL;
    cJSON *retrieved_created_number = NULL;
    cJSON *retrieved_added_number = NULL;
    const char *message_text = NULL;
    double created_num = 0.0;
    double added_num = 0.0;
    double total = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "added_number", added_val);
    cJSON_AddItemToObject(root, "created_number", num_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "missing", null_item);
    cJSON_AddStringToObject(child, "message", "hello cJSON");

    // step 3: Operate / Validate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_message = cJSON_GetObjectItem(retrieved_child, "message");
    message_text = cJSON_GetStringValue(retrieved_message);
    retrieved_created_number = cJSON_GetObjectItem(root, "created_number");
    retrieved_added_number = cJSON_GetObjectItem(root, "added_number");
    created_num = cJSON_GetNumberValue(retrieved_created_number);
    added_num = cJSON_GetNumberValue(retrieved_added_number);
    total = created_num + added_num;
    cJSON_AddNumberToObject(root, "sum", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)message_text;
    (void)created_num;
    (void)added_num;
    (void)total;
    return 66;
}