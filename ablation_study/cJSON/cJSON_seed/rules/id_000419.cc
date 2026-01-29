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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON *name = cJSON_CreateString("example");
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_flag = NULL;
    cJSON *retr_name = NULL;
    const char *name_str = NULL;
    cJSON_bool flag_invalid = 0;
    cJSON_bool name_invalid = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_flag = cJSON_GetObjectItem(parsed, "flag");
    retr_name = cJSON_GetObjectItem(parsed, "name");
    flag_invalid = cJSON_IsInvalid(retr_flag);
    name_invalid = cJSON_IsInvalid(retr_name);
    name_str = cJSON_GetStringValue(retr_name);
    buffer[0] = name_str[0];
    buffer[1] = (char)('0' + (int)flag_invalid);
    buffer[2] = (char)('0' + (int)name_invalid);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}