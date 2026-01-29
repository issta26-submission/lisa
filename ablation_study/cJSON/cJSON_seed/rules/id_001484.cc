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
//<ID> 1484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *parsed = NULL;
    cJSON_bool equal = 0;
    char json_buf[128] = "{ \"alpha\"  : true , \"beta\" : null }";

    // step 2: Setup / Configure
    cJSON_Minify(json_buf);
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "alpha", true_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "beta", null_item);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(json_buf);
    equal = cJSON_Compare(root, parsed, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}