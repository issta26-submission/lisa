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
//<ID> 926
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *flag = NULL;
    cJSON *got_name = NULL;
    cJSON *got_flag = NULL;
    cJSON *detached_name = NULL;
    cJSON *detached_flag = NULL;
    const char *literal = "example_string";
    const char *got_name_str = NULL;
    cJSON_bool is_flag_true = 0;
    char *buffer = NULL;
    const int buf_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString(literal);
    flag = cJSON_CreateTrue();
    cJSON_AddItemReferenceToObject(root, "name", name);
    cJSON_AddItemReferenceToObject(root, "flag", flag);

    // step 3: Operate / Validate
    got_name = cJSON_GetObjectItem(root, "name");
    got_flag = cJSON_GetObjectItem(root, "flag");
    got_name_str = cJSON_GetStringValue(got_name);
    is_flag_true = cJSON_IsTrue(got_flag);
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    buffer[0] = (char)('0' + (int)is_flag_true);
    buffer[1] = got_name_str[0];

    // step 4: Cleanup
    detached_name = cJSON_DetachItemFromObject(root, "name");
    detached_flag = cJSON_DetachItemFromObject(root, "flag");
    cJSON_Delete(root);
    cJSON_Delete(detached_name);
    cJSON_Delete(detached_flag);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}