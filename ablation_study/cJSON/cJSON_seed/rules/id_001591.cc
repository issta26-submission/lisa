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
//<ID> 1591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *nul = NULL;
    cJSON *flag = NULL;
    cJSON *tmp = NULL;
    char *out1 = NULL;
    char *out2 = NULL;
    cJSON_bool flag_is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    nul = cJSON_CreateNull();
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "nothing", nul);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    out1 = cJSON_PrintUnformatted(root);
    tmp = cJSON_GetObjectItem(root, "flag");
    flag_is_false = cJSON_IsFalse(tmp);
    cJSON_DeleteItemFromObject(root, "nothing");
    out2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out1);
    cJSON_free(out2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}