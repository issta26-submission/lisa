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
//<ID> 683
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    const char *version = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *extra = NULL;
    cJSON *num = NULL;

    // step 2: Setup / Configure
    child = cJSON_AddObjectToObject(root, "child");
    extra = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate / Validate
    num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "value", num);
    version = cJSON_Version();
    size_t vlen = version ? strlen(version) : 0;
    size_t to_copy = vlen < 255 ? vlen : 255;
    memcpy(scratch, version ? version : "", to_copy);
    scratch[to_copy < 255 ? to_copy : 255] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}