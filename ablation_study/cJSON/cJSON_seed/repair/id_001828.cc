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
//<ID> 1828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddNumberToObject(metrics, "count", 5.0);
    cJSON_AddNumberToObject(metrics, "ratio", 2.5);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddNumberToObject(root, "added", 123.456);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "origin", cJSON_CreateString("unit-test"));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *addedItem = cJSON_GetObjectItem(root, "added");
    cJSON *metricsItem = cJSON_GetObjectItem(root, "metrics");
    cJSON *ratioItem = cJSON_GetObjectItem(metricsItem, "ratio");
    double addedVal = cJSON_GetNumberValue(addedItem);
    double ratioVal = cJSON_GetNumberValue(ratioItem);
    const char *labelStr = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    buffer[0] = ver ? ver[0] : 'V';
    buffer[1] = labelStr ? labelStr[0] : 'L';
    buffer[2] = (char)('0' + (((int)addedVal) % 10));
    buffer[3] = (char)('0' + (((int)ratioVal) % 10));
    (void)buffer; (void)addedVal; (void)ratioVal; (void)labelStr; (void)ver; (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}