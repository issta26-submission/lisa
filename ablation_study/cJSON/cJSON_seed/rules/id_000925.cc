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
//<ID> 925
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
    cJSON *got_str = NULL;
    cJSON *got_true = NULL;
    cJSON *det_str = NULL;
    cJSON *det_true = NULL;
    const char *retrieved_str = NULL;
    cJSON_bool is_true = 0;
    char *buffer = NULL;
    size_t buf_len = 32;

    // step 2: Setup / Configure
    str_item = cJSON_CreateString("example_value");
    true_item = cJSON_CreateTrue();
    root = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(root, "my_string", str_item);
    cJSON_AddItemReferenceToObject(root, "my_flag", true_item);

    // step 3: Operate / Validate
    got_str = cJSON_GetObjectItem(root, "my_string");
    got_true = cJSON_GetObjectItem(root, "my_flag");
    retrieved_str = cJSON_GetStringValue(got_str);
    is_true = cJSON_IsTrue(got_true);
    buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = retrieved_str[0];

    // step 4: Cleanup
    det_str = cJSON_DetachItemFromObject(root, "my_string");
    det_true = cJSON_DetachItemFromObject(root, "my_flag");
    cJSON_Delete(det_str);
    cJSON_Delete(det_true);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}