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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *ver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(meta, "version", ver);

    // step 2: Configure
    const char *ver_str = cJSON_GetStringValue(ver);
    cJSON *ver_copy = cJSON_CreateString(ver_str);
    cJSON_AddItemToObjectCS(root, "version_copy", ver_copy);
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "data", data);

    // step 3: Operate
    cJSON_AddItemToObjectCS(data, "n1", cJSON_CreateNumber(1));
    cJSON_AddItemToObjectCS(data, "n2", cJSON_CreateNumber(2));
    cJSON_AddItemToObjectCS(data, "n3", cJSON_CreateNumber(3));
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}