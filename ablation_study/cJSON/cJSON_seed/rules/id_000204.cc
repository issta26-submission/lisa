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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *msg = cJSON_CreateString("hello world");
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);
    double set_result = 0.0;
    cJSON *flag_node = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_msg = NULL;
    double retrieved_value = 0.0;
    const char *retrieved_text = NULL;
    cJSON_bool flag_is_false = 0;
    (void)buffer;

    // step 2: Setup / Configure
    set_result = cJSON_SetNumberHelper(num, 3.14159);
    cJSON_AddItemToObject(root, "pi", num);
    flag_node = cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddItemToObject(root, "greeting", msg);

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItemCaseSensitive(root, "pi");
    retrieved_value = cJSON_GetNumberValue(retrieved_num);
    retrieved_msg = cJSON_GetObjectItem(root, "greeting");
    retrieved_text = cJSON_GetStringValue(retrieved_msg);
    flag_is_false = cJSON_IsFalse(flag_node);
    cJSON_PrintPreallocated(root, (char *)buffer, 256, 0);
    (void)set_result;
    (void)retrieved_value;
    (void)retrieved_text;
    (void)flag_is_false;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}