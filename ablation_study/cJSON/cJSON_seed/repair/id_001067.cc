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
//<ID> 1067
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *always_on = cJSON_AddTrueToObject(root, "alwaysOn");

    // step 2: Configure
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, flag_false);
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddStringToObject(root, "label", "example");

    // step 3: Operate and Validate
    double is_obj = (double)cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "is_object", is_obj);
    char *flat = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (int)is_obj);
    ((char *)scratch)[1] = (char)('0' + (flat != NULL));
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}