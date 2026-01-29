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
//<ID> 1330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"pi\":3.1415,\"msg\":\"hello\"}";
    cJSON *root = NULL;
    cJSON *pi_item = NULL;
    cJSON *msg_item = NULL;
    cJSON *pi_times2 = NULL;
    cJSON *msg_copy = NULL;
    cJSON *dup = NULL;
    cJSON *pi_times2_check = NULL;
    cJSON *msg_copy_check = NULL;
    double pi_val = 0.0;
    double pi_times2_val = 0.0;
    const char *msg_str = NULL;
    const char *msg_copy_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    pi_item = cJSON_GetObjectItem(root, "pi");
    msg_item = cJSON_GetObjectItem(root, "msg");

    // step 3: Operate / Validate
    pi_val = cJSON_GetNumberValue(pi_item);
    pi_times2 = cJSON_CreateNumber(pi_val * 2.0);
    msg_copy = cJSON_CreateString("hello_copy");
    cJSON_AddItemToObject(root, "pi_times2", pi_times2);
    cJSON_AddItemToObject(root, "msg_copy", msg_copy);
    dup = cJSON_Duplicate(root, 1);
    pi_times2_check = cJSON_GetObjectItem(dup, "pi_times2");
    msg_copy_check = cJSON_GetObjectItem(dup, "msg_copy");
    pi_times2_val = cJSON_GetNumberValue(pi_times2_check);
    msg_str = cJSON_GetStringValue(msg_item);
    msg_copy_str = cJSON_GetStringValue(msg_copy_check);
    (void)pi_times2_val;
    (void)msg_str;
    (void)msg_copy_str;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}