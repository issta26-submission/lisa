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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *flag_item = NULL;
    cJSON *repl_str = cJSON_CreateString("replaced");
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_num = NULL;
    const char *got_str = NULL;
    cJSON_bool rc_add = 0;
    cJSON_bool rc_replace = 0;

    // step 2: Setup / Configure
    flag_item = cJSON_AddTrueToObject(root, "flag");
    rc_add = cJSON_AddItemToObject(root, "num", num);
    rc_replace = cJSON_ReplaceItemInObject(root, "num", repl_str);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    got_flag = cJSON_GetObjectItem(parsed, "flag");
    got_num = cJSON_GetObjectItem(parsed, "num");
    got_str = cJSON_GetStringValue(got_num);
    buffer[0] = (char)('0' + (int)cJSON_IsTrue(got_flag));
    buffer[1] = got_str[0];
    buffer[2] = (char)('0' + (int)rc_add);
    buffer[3] = (char)('0' + (int)rc_replace);
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}