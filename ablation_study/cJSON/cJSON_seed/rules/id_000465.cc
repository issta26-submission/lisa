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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *text = cJSON_CreateString("hello");
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON_bool replace_result = 0;
    cJSON_bool add_result = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddItemToObject(root, "msg", text);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    cJSON *replacement = cJSON_CreateString("world");
    replace_result = cJSON_ReplaceItemInObject(parsed, "msg", replacement);
    cJSON *extra = cJSON_CreateNumber(2.718);
    add_result = cJSON_AddItemToObject(parsed, "e", extra);
    const char *got_text = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "msg"));
    double got_num = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "num"));
    int active_bool = cJSON_IsTrue(cJSON_GetObjectItem(parsed, "active"));
    buffer[0] = (char)('A' + ((int)got_num % 26));
    buffer[1] = (char)('0' + (active_bool % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}