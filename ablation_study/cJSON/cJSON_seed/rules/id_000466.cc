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
//<ID> 466
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
    cJSON *nameItem = cJSON_CreateString("example");
    cJSON *numItem = cJSON_CreateNumber(123.0);
    cJSON *newNum = cJSON_CreateNumber(42.0);
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *got_name = NULL;
    cJSON *got_num = NULL;
    double num_val = 0.0;
    char *name_val = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", nameItem);
    cJSON_AddTrueToObject(root, "flag");
    cJSON_AddItemToObject(root, "num", numItem);
    replaced = cJSON_ReplaceItemInObject(root, "num", newNum);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    got_name = cJSON_GetObjectItem(parsed, "name");
    got_num = cJSON_GetObjectItem(parsed, "num");
    name_val = cJSON_GetStringValue(got_name);
    num_val = cJSON_GetNumberValue(got_num);
    buffer[0] = (char)('0' + ((int)num_val % 10));
    buffer[1] = name_val[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}