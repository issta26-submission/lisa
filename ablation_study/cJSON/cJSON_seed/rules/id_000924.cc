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
//<ID> 924
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
    cJSON *retrieved_str = NULL;
    cJSON *retrieved_true = NULL;
    const char *literal = "hello_world";
    const char *retrieved_cstr = NULL;
    cJSON_bool flag = 0;
    char *buffer = NULL;
    const int BUF_LEN = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(literal);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemReferenceToObject(root, "message", str_item);
    cJSON_AddItemReferenceToObject(root, "active", true_item);

    // step 3: Operate / Validate
    retrieved_str = cJSON_GetObjectItemCaseSensitive(root, "message");
    retrieved_true = cJSON_GetObjectItemCaseSensitive(root, "active");
    retrieved_cstr = cJSON_GetStringValue(retrieved_str);
    flag = cJSON_IsTrue(retrieved_true);
    buffer = (char *)cJSON_malloc((size_t)BUF_LEN);
    memset(buffer, 0, (size_t)BUF_LEN);
    buffer[0] = retrieved_cstr[0];
    buffer[1] = (char)('0' + (int)flag);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}