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
//<ID> 923
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *message = NULL;
    cJSON *flag = NULL;
    cJSON *flag_ref = NULL;
    const char *msg = "hello_ref";
    cJSON_bool is_flag_ref_true = 0;
    char *buffer = NULL;
    const size_t buf_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    message = cJSON_CreateString(msg);
    cJSON_AddItemToObject(root, "message", message);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemReferenceToObject(root, "flag_ref", flag);

    // step 3: Operate / Validate
    flag_ref = cJSON_GetObjectItem(root, "flag_ref");
    is_flag_ref_true = cJSON_IsTrue(flag_ref);
    buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    buffer[0] = (char)('0' + (int)is_flag_ref_true);
    (void)cJSON_GetStringValue(cJSON_GetObjectItem(root, "message"));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}