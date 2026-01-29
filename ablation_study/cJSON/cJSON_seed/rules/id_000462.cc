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
//<ID> 462
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
    cJSON *nameItem = cJSON_CreateString("original");
    cJSON *replacement = cJSON_CreateString("newvalue");
    cJSON *parsed = NULL;
    char *printed = NULL;
    const char *name_in_parsed = NULL;
    cJSON *nameParsedItem = NULL;
    cJSON *flagParsedItem = NULL;
    cJSON_bool replaced_ok = 0;
    cJSON_bool is_true_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddTrueToObject(root, "flag");
    cJSON_AddItemToObject(root, "name", nameItem);
    replaced_ok = cJSON_ReplaceItemInObject(root, "name", replacement);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    nameParsedItem = cJSON_GetObjectItem(parsed, "name");
    flagParsedItem = cJSON_GetObjectItem(parsed, "flag");
    name_in_parsed = cJSON_GetStringValue(nameParsedItem);
    is_true_flag = cJSON_IsTrue(flagParsedItem);
    buffer[0] = name_in_parsed[0];
    buffer[1] = (char)('0' + (int)is_true_flag);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}