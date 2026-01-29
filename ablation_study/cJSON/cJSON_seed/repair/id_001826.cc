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
//<ID> 1826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(data, "count", 7.0);
    cJSON_AddNumberToObject(data, "ratio", 0.25);
    cJSON *nameItem = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", nameItem);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *verStr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", verStr);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t bufsize = 128;
    char *buffer = (char *)cJSON_malloc(bufsize);
    memset(buffer, 0, bufsize);
    cJSON *dataObj = cJSON_GetObjectItem(root, "data");
    cJSON *countItem = cJSON_GetObjectItem(dataObj, "count");
    double countVal = cJSON_GetNumberValue(countItem);
    const char *nameStr = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = nameStr ? nameStr[0] : 'Z';
    buffer[2] = (char)('0' + (((int)countVal) % 10));
    buffer[3] = ver ? ver[0] : 'V';
    (void)buffer; (void)nameStr; (void)countVal; (void)ver;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}