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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *flag_node = NULL;
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    double set_result = 0.0;
    double read_value = 0.0;
    cJSON_bool flag_is_false = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    flag_node = cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(num, 3.1415);
    read_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    flag_is_false = cJSON_IsFalse(flag_node);
    ((char *)buffer)[0] = ver[0];
    (void)set_result;
    (void)read_value;
    (void)flag_is_false;
    (void)ver;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}