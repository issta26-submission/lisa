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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON *str = cJSON_CreateString("example");
    const char *ver = cJSON_Version();
    char *buf = (char *)cJSON_malloc(16);
    memset(buf, 0, 16);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "meta", child);
    cJSON_AddItemToObject(root, "name", str);

    // step 3: Operate / Validate
    double v = cJSON_GetNumberValue(num);
    cJSON *dbl = cJSON_CreateNumber(v * 2.0);
    cJSON_AddItemToObject(child, "double", dbl);
    cJSON_bool name_is_string = cJSON_IsString(str);
    buf[0] = ver ? ver[0] : '\0';
    (void)name_is_string;
    (void)v;
    (void)buf;

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}