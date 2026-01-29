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
//<ID> 643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    char *pretty = NULL;
    char *compact = NULL;
    size_t len_pretty = 0;
    size_t len_compact = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *label_str = cJSON_CreateString("primary-label");

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddRawToObject(root, "meta", "{\"version\":1,\"active\":true}");
    cJSON_AddItemToObject(root, "label", label_str);

    // step 3: Operate / Validate
    pretty = cJSON_Print(root);
    len_pretty = pretty ? strlen(pretty) : 0;
    to_copy = len_pretty < 1023 ? len_pretty : 1023;
    memcpy(scratch, pretty ? pretty : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(pretty);

    compact = cJSON_PrintUnformatted(root);
    len_compact = compact ? strlen(compact) : 0;
    to_copy = len_compact < 1023 ? len_compact : 1023;
    memcpy(scratch, compact ? compact : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(compact);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}