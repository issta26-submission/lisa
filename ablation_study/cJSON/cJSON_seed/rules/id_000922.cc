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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *true_item = NULL;
    cJSON *retrieved_str_item = NULL;
    cJSON *retrieved_true_item = NULL;
    const char *greeting = "hello_cJSON";
    char *buffer = NULL;
    char *retrieved_str = NULL;
    cJSON_bool flag_is_true = 0;
    cJSON_bool add_ref_res1 = 0;
    cJSON_bool add_ref_res2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(greeting);
    true_item = cJSON_CreateTrue();
    add_ref_res1 = cJSON_AddItemReferenceToObject(root, "greeting", str_item);
    add_ref_res2 = cJSON_AddItemReferenceToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    retrieved_str_item = cJSON_GetObjectItem(root, "greeting");
    retrieved_true_item = cJSON_GetObjectItem(root, "enabled");
    retrieved_str = cJSON_GetStringValue(retrieved_str_item);
    flag_is_true = cJSON_IsTrue(retrieved_true_item);
    buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    buffer[0] = (char)('0' + (int)(flag_is_true ? 1 : 0));
    buffer[1] = (retrieved_str && retrieved_str[0]) ? retrieved_str[0] : 'x';
    (void)add_ref_res1;
    (void)add_ref_res2;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}