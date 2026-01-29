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
//<ID> 411
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
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON *invalid = (cJSON *)cJSON_malloc(sizeof(struct cJSON));
    invalid->next = NULL;
    invalid->prev = NULL;
    invalid->child = NULL;
    invalid->type = 0; /* mark as invalid (cJSON_Invalid == 0 in cJSON implementations) */
    invalid->valuestring = NULL;
    invalid->valueint = 0;
    invalid->valuedouble = 0.0;
    invalid->string = NULL;
    cJSON_bool init_invalid_flag = cJSON_IsInvalid(invalid);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "enabled", bool_item);
    cJSON_AddItemToObject(root, "broken", invalid);
    cJSON_bool add_enabled_ok = cJSON_AddItemToObject(root, "duplicate_check", cJSON_CreateBool(0));

    // step 3: Operate / Validate
    cJSON *retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *retrieved_broken = cJSON_GetObjectItem(root, "broken");
    cJSON_bool retrieved_invalid_flag = cJSON_IsInvalid(retrieved_broken);
    char *printed = cJSON_PrintUnformatted(root);
    tmp_buf[0] = (char)('0' + (int)init_invalid_flag);
    tmp_buf[1] = (char)('0' + (int)retrieved_invalid_flag);
    tmp_buf[2] = (char)('0' + (int)add_enabled_ok);
    tmp_buf[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}