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
//<ID> 528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *manual = cJSON_CreateString("manual_string");
    cJSON *added = NULL;
    cJSON_bool pre_ok = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    added = cJSON_AddStringToObject(root, "greeting", "Hello, World!");
    cJSON_AddItemToObject(root, "manual", manual);

    // step 3: Operate / Validate
    pre_ok = cJSON_PrintPreallocated(root, buffer, 512, 1);
    printed = cJSON_Print(root);
    buffer[510] = (char)('0' + (char)pre_ok);
    buffer[511] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}