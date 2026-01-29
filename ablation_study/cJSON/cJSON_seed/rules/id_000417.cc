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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *tmp_buf = (char *)cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *inner_flag = cJSON_CreateBool(0);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON_bool add_ok_root_flag = 0;
    cJSON_bool add_ok_root_nested = 0;
    cJSON_bool add_ok_nested_inner = 0;
    cJSON_bool is_invalid = 0;

    // step 2: Setup / Configure
    add_ok_root_flag = cJSON_AddItemToObject(root, "flag", flag);
    add_ok_root_nested = cJSON_AddItemToObject(root, "nested", nested);
    add_ok_nested_inner = cJSON_AddItemToObject(nested, "inner", inner_flag);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse((const char *)printed);
    retrieved_flag = cJSON_GetObjectItem(parsed, "flag");
    is_invalid = cJSON_IsInvalid(retrieved_flag);
    tmp_buf[0] = (char)('0' + (int)add_ok_root_flag);
    tmp_buf[1] = (char)('0' + (int)add_ok_root_nested);
    tmp_buf[2] = (char)('0' + (int)add_ok_nested_inner);
    tmp_buf[3] = (char)('0' + (int)is_invalid);
    tmp_buf[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}