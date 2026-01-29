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
//<ID> 1821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "inner", 7.0);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *verItem = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", verItem);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 3: Operate and Validate
    cJSON *gotVerItem = cJSON_GetObjectItem(root, "version");
    cJSON *gotCountItem = cJSON_GetObjectItem(root, "count");
    const char *verStr = cJSON_GetStringValue(gotVerItem);
    double countVal = cJSON_GetNumberValue(gotCountItem);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = verStr[0];
    buffer[1] = (char)('0' + (((int)countVal) % 10));
    (void)printed; (void)ver; (void)verItem; (void)gotVerItem; (void)gotCountItem; (void)countVal; (void)verStr; (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}