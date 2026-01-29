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
//<ID> 647
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
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *title = cJSON_CreateString("cJSON Test Suite");
    cJSON_AddItemToObject(info, "title", title);
    cJSON_AddStringToObject(root, "version", "1.2.3");
    cJSON_AddRawToObject(root, "meta", "{\"active\":true,\"count\":3}");

    // step 2: Setup / Configure
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    size_t compact_len = compact ? strlen(compact) : 0;
    size_t to_copy = compact_len < 1023 ? compact_len : 1023;
    memcpy(scratch, compact ? compact : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(compact);

    // step 3: Operate / Validate
    cJSON *backup = cJSON_CreateString(pretty ? pretty : "");
    cJSON_AddItemToObject(root, "backup", backup);
    cJSON_free(pretty);
    char *final_print = cJSON_PrintUnformatted(root);
    cJSON_free(final_print);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}