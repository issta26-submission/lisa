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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(100.0);
    cJSON *num2 = cJSON_CreateNumber(200.0);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *gotnum = NULL;
    double val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "subval", cJSON_CreateString("hello"));
    cJSON_AddItemToObject(root, "num", num1);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    cJSON_ReplaceItemInObject(parsed, "num", num2);
    cJSON_AddItemToObject(parsed, "added", cJSON_CreateString("addedstr"));
    gotnum = cJSON_GetObjectItem(parsed, "num");
    val = cJSON_GetNumberValue(gotnum);
    buffer[0] = (char)('0' + ((int)val % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}