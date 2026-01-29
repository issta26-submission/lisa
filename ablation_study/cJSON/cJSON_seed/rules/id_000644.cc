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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *pretty = NULL;
    char *compact = NULL;
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_str = cJSON_CreateString("created-via-CreateString");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created", created_str);
    cJSON_AddStringToObject(root, "inline", "inline-value");
    cJSON_AddRawToObject(root, "raw", "{\"nested\":123}");

    // step 3: Operate / Validate
    pretty = cJSON_Print(root);
    compact = cJSON_PrintUnformatted(root);
    size_t pretty_len = pretty ? strlen(pretty) : 0;
    size_t to_copy = pretty_len < 1023 ? pretty_len : 1023;
    memcpy(scratch, pretty ? pretty : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}