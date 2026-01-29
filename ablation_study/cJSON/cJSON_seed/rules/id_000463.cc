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
//<ID> 463
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *newitem = NULL;
    cJSON *retr_status = NULL;
    cJSON *retr_value = NULL;
    double value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddTrueToObject(root, "status");

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    newitem = cJSON_CreateString("ok");
    cJSON_ReplaceItemInObject(parsed, "status", newitem);
    retr_status = cJSON_GetObjectItem(parsed, "status");
    retr_value = cJSON_GetObjectItem(parsed, "value");
    value = cJSON_GetNumberValue(retr_value);
    buffer[0] = (char)('0' + ((int)value % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}