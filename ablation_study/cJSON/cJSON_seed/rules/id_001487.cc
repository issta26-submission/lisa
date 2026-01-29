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
//<ID> 1487
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
    cJSON *embedded = NULL;
    char json_buf[] = " { \"x\" : 1, \"y\" : true } ";
    char *out = NULL;

    // step 2: Setup / Configure
    cJSON_Minify(json_buf);
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "opt", null_item);
    embedded = cJSON_CreateString(json_buf);
    cJSON_AddItemToObject(root, "minified_source", embedded);

    // step 3: Operate / Validate
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}